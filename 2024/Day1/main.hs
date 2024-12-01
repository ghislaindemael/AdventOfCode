import System.IO
import Data.List
import Text.Read

hFetchLines :: Handle -> IO [String]
hFetchLines handle = do
    eof <- hIsEOF handle
    if eof
        then return []
        else do
            line <- hGetLine handle
            rest <- hFetchLines handle
            return (line : rest)

splitLine :: String -> (String, String)
splitLine line =
    let wordsList = words line
    in case wordsList of
        (x:y:_) -> (x, y)
        _        -> ("", "")

parseNumber :: String -> Maybe Int
parseNumber str = readMaybe str :: Maybe Int

countOccurrences :: Eq a => a -> [a] -> Int
countOccurrences x xs = length (filter (== x) xs)

main :: IO ()
main = do
    let fileName = "input.txt"
    handle <- openFile fileName ReadMode
    lines <- hFetchLines handle
    let (list1, list2) = unzip $ map splitLine lines
    let validList1 = mapM parseNumber list1
        validList2 = mapM parseNumber list2

    -- Part one
    case (validList1, validList2) of
        (Just nums1, Just nums2) -> do
            let sortedList1 = sort nums1
                sortedList2 = sort nums2
            let totalSum = sum (map abs (zipWith (-) sortedList1 sortedList2))
            print totalSum
        _ -> putStrLn "Error: Some values are not valid numbers."

    --Part two
    case (validList1, validList2) of
        (Just nums1, Just nums2) -> do
            let totalSum = sum (map (\x -> x * countOccurrences x nums2) nums1)
            print totalSum
        _ -> putStrLn "Error: Some values are not valid numbers."

    hClose handle
